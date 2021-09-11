#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/utils/AtlasBuilder.h"

#include "spritingsupport/spritecreator.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include <qvariant.h>
#include <qfile.h>

namespace oxygine
{

    constexpr qint32 defaultAtlasWidth = 2048;
    constexpr qint32 defaultAtlasHeight = 2048;

    struct atlas_data
    {
        spTexture texture;
        Image mt;
        AtlasBuilder atlas;
    };

    void ResAtlasGeneric::applyAtlas(atlas_data& ad, quint32 filter, bool clamp2edge)
    {
        if (ad.texture.get() == nullptr)
        {
            return;
        }
        spImage mt = spImage::create();
        Rect bounds = ad.atlas.getBounds();
        qint32 w = bounds.getRight();
        qint32 h = bounds.getBottom();

        qint32 aw = w % 4;
        aw = aw ? w + 4 - aw : w;

        ImageData reg = ad.mt.lock().getRect(Rect(0, 0, aw, h));
        mt->init(reg);
        ad.texture->init(mt->lock());
        ad.texture->setLinearFilter(filter);
        ad.texture->setClamp2Edge(clamp2edge);
    }

    void ResAtlasGeneric::nextAtlas(qint32 w, qint32 h, ImageData::TextureFormat tf, atlas_data& ad)
    {
        ad.mt.init(w, h, tf);
        ad.mt.fillZero();

        ad.atlas.clean();
        ad.atlas.init(w, h);
        if (m_atlasses.size() > m_current)
        {
            ad.texture = m_atlasses[m_current].base;
        }
        else
        {
            ad.texture = VideoDriver::instance->createTexture();

            atlas atl;
            atl.base = ad.texture;
            m_atlasses.push_back(atl);
        }
        m_current++;
    }

    void ResAtlasGeneric::_unload()
    {
    }

    void ResAtlasGeneric::_load()
    {
    }

    void ResAtlasGeneric::loadAtlas(CreateResourceContext& context)
    {
        loadAtlas2(context);
    }

    void ResAtlasGeneric::loadAtlas2(CreateResourceContext& context)
    {
        m_current = 0;
        QDomElement node = context.m_walker.getNode();

        bool ok = false;
        qint32 w = node.attribute("width").toInt(&ok);
        if (!ok)
        {
            w = defaultAtlasWidth;
        }
        qint32 h = node.attribute("height").toInt(&ok);
        if (!ok)
        {
            h = defaultAtlasHeight;
        }

        loadBase(node);

        atlas_data ad;

        ImageData::TextureFormat tf = ImageData::TF_R8G8B8A8;
        QVector<spResAnim> anims;
        while (true)
        {
            XmlWalker walker = context.m_walker.next();
            if (walker.empty())
            {
                break;
            }

            QDomElement child_node = walker.getNode();

            QString name = child_node.nodeName();
            if (name != "image")
            {
                continue;
            }

            QString file = child_node.attribute("file");

            if (file.isEmpty())
            {
                createEmpty(walker, context);
                continue;
            }
            bool found = false;
            for (const auto& item : anims)
            {
                if (item->getResPath() == walker.getPath("file"))
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                Console::print("Duplicate entry found. " + walker.getPath("file"), Console::eFATAL);
                continue;
            }

            QVariant value = QVariant(child_node.attribute("trim"));
            bool trim = true;
            if (value.typeId() == QMetaType::Bool)
            {
                trim = value.toBool();
            }
            value = QVariant(child_node.attribute("extend"));
            bool extend = true;
            if (value.typeId() == QMetaType::Bool)
            {
                extend = value.toBool();
            }

            Point offset = extend ? Point(2, 2) : Point(0, 0);

            Image mt;
            ImageData im;

            qint32 columns = 0;
            qint32 rows = 0;
            qint32 frame_width = 0;
            qint32 frame_height = 0;

            QImage img;
            if (QFile::exists(Settings::getUserPath() + walker.getPath("file")))
            {
                img = QImage(Settings::getUserPath() + walker.getPath("file"));
            }
            else if (QFile::exists(RCC_PREFIX_PATH + walker.getPath("file")))
            {
                img = QImage(RCC_PREFIX_PATH + walker.getPath("file"));
            }
            else
            {
                Console::print("Invalid item found. " + walker.getPath("file"), Console::eFATAL);
                continue;
            }
            if (img.width() == 0 || img.height() == 0)
            {
                Console::print("Image is not valid " + walker.getPath("file"), Console::eWARNING);
                continue;
            }
            rows = child_node.attribute("rows").toInt();
            frame_width = child_node.attribute("frame_width").toInt();
            columns = child_node.attribute("cols").toInt();
            frame_height = child_node.attribute("frame_height").toInt();

            float scaleFactor = child_node.attribute("scale_factor").toFloat(&ok);
            if (!ok)
            {
                scaleFactor = 1.0f;
            }
            bool maskExtend = true;
            if (child_node.hasAttribute("maskExtend"))
            {
                QVariant varMaskExtend = QVariant(child_node.attribute("maskExtend"));
                if (varMaskExtend.typeId() == QMetaType::QString &&
                    !varMaskExtend.isNull() &&
                    child_node.hasAttribute("maskExtend"))
                {
                    maskExtend = value.toBool();
                }
            }
            if (rows <= 0)
            {
                rows = 1;
            }
            if (columns <= 0)
            {
                columns = 1;
            }
            QString path = walker.getPath("file");
            if (path.endsWith("+mask.png") && maskExtend)
            {
                QString basePath = path.replace("+mask.png", ".png");
                QString overlayPath = basePath;
                if (QFile::exists(Settings::getUserPath() + basePath))
                {
                    overlayPath = Settings::getUserPath() + basePath;
                }
                else
                {
                    overlayPath = oxygine::Resource::RCC_PREFIX_PATH + basePath;
                }
                if (QFile::exists(overlayPath))
                {
                    Console::print("SpriteCreator::preProcessMask for file " + path, Console::eDEBUG);
                    QImage overlay(overlayPath);
                    SpriteCreator::preProcessMask(img, overlay, columns, rows);
                }
            }
            SpriteCreator::addFrameBorders(img, columns, rows, context.m_addTransparentBorder);
            if (frame_width > 0)
            {
                columns = img.width() / frame_width;
            }
            else
            {
                frame_width = img.width() / columns;
            }
            if (frame_height > 0)
            {
                rows = img.height() / frame_height;
            }
            else
            {
                frame_height = img.height() / rows;
            }
            animationFrames frames;
            qint32 frames_count = rows * columns;
            frames.reserve(frames_count);
            qint32 width = frame_width;
            qint32 height = frame_height;
            if (rows > 1 || columns > 1)
            {
                if (context.m_addTransparentBorder)
                {
                    frame_width -= 1;
                    frame_height -= 1;
                }
                else
                {
                    frame_width -= 2;
                    frame_height -= 2;
                }
            }
            mt.init(img, true);
            im = mt.lock();

            spResAnim ra = spResAnim::create(this);
            ra->setResPath(walker.getPath("file"));

            anims.push_back(ra);

            for (qint32 y = 0; y < rows; ++y)
            {
                for (qint32 x = 0; x < columns; ++x)
                {
                    Rect frameRect;
                    frameRect.pos = Point(x * width, y * height);
                    frameRect.size = Point(frame_width, frame_height);

                    ImageData srcImage_ = im.getRect(frameRect);


                    HitTestData adata;
                    ImageData src;
                    Rect bounds(0, 0, srcImage_.m_w, srcImage_.m_h);
                    if (trim)
                    {
                        Image::makeAlpha(srcImage_, bounds, m_hitTestBuffer, adata, walker.getAlphaHitTest());
                    }
                    src = srcImage_.getRect(bounds);

                    Rect dest(0, 0, 0, 0);

                    if (ad.texture.get() == nullptr)
                    {
                        nextAtlas(w, h, tf, ad);
                    }

                    bool s = ad.atlas.add(&ad.mt, src, dest, offset);
                    if (s == false)
                    {
                        applyAtlas(ad, m_linearFilter, m_clamp2edge);

                        nextAtlas(w, h, tf, ad);
                        s = ad.atlas.add(&ad.mt, src, dest, offset);
                        if (s == false)
                        {
                            oxygine::handleErrorPolicy(oxygine::ep_show_error, "ResAtlasGeneric::loadAtlas2 adding to atlas failed");
                        }
                    }

                    //extend = false;
                    if (extend)
                    {
                        //duplicate image edges
                        Image& mt = ad.mt;
                        ImageData tmp;

                        if (bounds.getY() == 0 && dest.pos.y != 0)
                        {
                            tmp = mt.lock(Rect(dest.pos.x, dest.pos.y - 1, src.m_w, 1));
                            operations::copy(src.getRect(Rect(0, 0, src.m_w, 1)), tmp);
                        }

                        if (bounds.getHeight() == im.m_h && dest.getBottom() != mt.getHeight())
                        {
                            tmp = mt.lock(Rect(dest.pos.x, dest.pos.y + src.m_h, src.m_w, 1));
                            operations::copy(src.getRect(Rect(0, src.m_h - 1, src.m_w, 1)), tmp);
                        }

                        if (bounds.getX() == 0 && dest.pos.x != 0)
                        {
                            tmp = mt.lock(Rect(dest.pos.x - 1, dest.pos.y, 1, src.m_h));
                            operations::copy(src.getRect(Rect(0, 0, 1, src.m_h)), tmp);
                        }

                        if (bounds.getWidth() == im.m_w && dest.getRight() != mt.getWidth())
                        {
                            tmp = mt.lock(Rect(dest.pos.x + src.m_w, dest.pos.y, 1, src.m_h));
                            operations::copy(src.getRect(Rect(src.m_w - 1, 0, 1, src.m_h)), tmp);
                        }
                    }
                    float iw = 1.0f;
                    float ih = 1.0f;

                    RectF srcRect(dest.pos.x * iw, dest.pos.y * ih, dest.size.x * iw, dest.size.y * ih);

                    Vector2 sizeScaled = Vector2((float)dest.size.x, (float)dest.size.y) * walker.getScaleFactor() * scaleFactor;
                    RectF destRect(bounds.pos.cast<Vector2>(), sizeScaled);

                    AnimationFrame frame;
                    Diffuse df;
                    df.base = ad.texture;

                    Vector2 fsize = Vector2((float)frame_width, (float)frame_height) * walker.getScaleFactor() * scaleFactor;
                    frame.init2(ra.get(), x, y, df,
                                srcRect, destRect, fsize);

                    frame.setHitTestData(adata);

                    frames.push_back(frame);
                }
            }

            init_resAnim(ra, file, child_node);

            ra->init(frames, columns, walker.getScaleFactor() * scaleFactor, 1.0f / (walker.getScaleFactor() * scaleFactor));
            ra->setParent(this);
            context.m_resources->add(ra);
        }

        applyAtlas(ad, m_linearFilter, m_clamp2edge);

        for (QVector<spResAnim>::iterator i = anims.begin(); i != anims.end(); ++i)
        {
            spResAnim rs = *i;
            qint32 num = rs->getTotalFrames();

            for (qint32 n = 0; n < num; ++n)
            {
                AnimationFrame& frame = const_cast<AnimationFrame&>(rs->getFrame(n));

                float iw = 1.0f / frame.getDiffuse().base->getWidth();
                float ih = 1.0f / frame.getDiffuse().base->getHeight();

                RectF rect = frame.getSrcRect();
                rect.pos.x *= iw;
                rect.pos.y *= ih;
                rect.size.x *= iw;
                rect.size.y *= ih;
                frame.setSrcRect(rect);

                HitTestData ad = frame.getHitTestData();
                if (ad.pitch)
                {
                    ad.data = &m_hitTestBuffer[reinterpret_cast<size_t>(ad.data)];
                    frame.setHitTestData(ad);
                }
            }
        }
    }

}
